import java.util.NoSuchElementException;
public class Main{
    public static void main(String[] argvs){
        Queue<Integer> que = new Queue<Integer>();

        int i;
        boolean flag = false;

        // add test
        for(i = 0; i < que.dump; ++i)
            assert que.add(i);
        System.out.println("[x] insert 1");

        for(i = 0; i < que.dump; ++i)
            assert que.add(i);
        System.out.println("[x] insert 2");

        flag = false;
        try{
            for(i = 0; i < que.dump; ++i)
                que.add(i);
        } catch (IllegalStateException e) {
            System.out.println("[x] insert 3");
            flag = true;
        }
        assert flag;

        // offer test
        que = new Queue<Integer>();
        for(i = 0; i < que.dump; ++i)
            assert que.offer(i);
        System.out.println("[x] offer 1");

        for(i = 0; i < que.dump; ++i)
            assert que.offer(i);
        System.out.println("[x] offer 2");

        for(i = 0; i < que.dump; ++i)
            assert !que.offer(i);
        System.out.println("[x] offer 3");

        // remove test
        for(i = 0; i < que.dump; ++i)
            assert que.remove() == i;
        System.out.println("[x] remove 1");

        for(i = 0; i < que.dump; ++i)
            assert que.remove() == i;
        System.out.println("[x] remove 2");

        flag = false;
        try {
            for(i = 0; i < que.dump; ++i)
                que.remove();
        } catch (NoSuchElementException e) {
            System.out.println("[x] remove 3");
            flag = true;
        }
        assert flag;

        // poll test
        for(i = 0; i < 2 * que.dump; ++i)
            assert que.add(i);
        System.out.println("[x] poll 1");

        for(i = 0; i < 2 * que.dump; ++i)
            assert que.poll() == i;
        System.out.println("[x] poll 2");

        for(i = 0; i < que.dump; ++i)
            assert que.poll() == null;
        System.out.println("[x] poll 3");

        // element test
        que = new Queue<Integer>();
        for(i = 0; i < que.dump + 1; ++i){
            que.add(i);
            assert que.element() == 0;
        }
        System.out.println("[x] element 1");

        for(i = 0; i < que.dump + 1; ++i){
            assert que.element() == i;
            que.remove();
        }
        System.out.println("[x] element 2");

        flag = false;
        try{
            que.element();
        } catch (NoSuchElementException e) {
            System.out.println("[x] element 3");
            flag = true;
        }
        assert flag;

        // peek test
        que = new Queue<Integer>();
        for(i = 0; i < que.dump + 1; ++i){
            que.add(i);
            assert que.peek() == 0;
        }
        System.out.println("[x] peek 1");

        for(i = 0; i < que.dump + 1; ++i){
            assert que.peek() == i;
            que.remove();
        }
        System.out.println("[x] peek 2");

        assert que.peek() == null;
        System.out.println("[x] peek 3");
    }
}
