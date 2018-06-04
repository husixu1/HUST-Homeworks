import java.util.Stack;
import java.util.NoSuchElementException;
public class Queue<E> extends Stack<E> {
    public final int dump = 10;
    private  Stack<E> stk;
    public Queue() {
        stk = new Stack<E>();
    }

    public boolean add(E e) throws IllegalStateException, ClassCastException, NullPointerException, IllegalArgumentException {
        if(e == null)
            throw new NullPointerException();
        if (stk.size() >= dump){
            if(!this.isEmpty())
                throw new IllegalStateException();
            else {
                while(!stk.isEmpty())
                    this.push(stk.pop());
                stk.push(e);
            }
        } else {
            stk.push(e);
        }
        return true;
    }

    public boolean offer(E e) throws ClassCastException, NullPointerException, IllegalArgumentException {
        if(e == null)
            throw new NullPointerException();
        if (stk.size() >= dump){
            if(!this.isEmpty())
                return false;
            else{
                while(!stk.isEmpty())
                    this.push(stk.pop());
                stk.push(e);
            }
        } else {
            stk.push(e);
        }
        return true;
    }
    public E remove() throws NoSuchElementException {
        if(!this.isEmpty()){
            return this.pop();
        } else if(!stk.isEmpty()){
            while(!stk.isEmpty())
                this.push(stk.pop());
            return this.pop();
        } else {
            throw new NoSuchElementException();
        }
    }

    public E poll() {
        if(!this.isEmpty()){
            return this.pop();
        } else if(!stk.isEmpty()){
            while(!stk.isEmpty())
                this.push(stk.pop());
            return this.pop();
        } else {
            return null;
        }
    }

    public E element() throws NoSuchElementException {
        if(!this.isEmpty()){
            return super.peek();
        } else if(!stk.isEmpty()){
            while(!stk.isEmpty())
                this.push(stk.pop());
            return super.peek();
        } else {
            throw new NoSuchElementException();
        }
    }

    public E peek() {
        if(!this.isEmpty()){
            return super.peek();
        } else if(!stk.isEmpty()){
            while(!stk.isEmpty())
                this.push(stk.pop());
            return super.peek();
        } else {
            return null;
        }
    }
}
